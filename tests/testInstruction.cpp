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
//File: testInstruction.cpp
//Content: test for the module Instruction
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testInstruction.h"
#include "../src/Instruction.h"
//#include "cppunit_assert_emulator.h" //Qt only.

#include <iostream>
#include <limits.h> //INT_MAX
#include <float.h> //DBL_MAX

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestInstruction);

//overide setUp(), init data, etc
void TestInstruction::setUp() {
    I = new TInstruction();
}

//overide tearDown(), free allocated memory, etc
void TestInstruction::tearDown() {
    delete I;
}

//---------------------------------------------------------------------------

void TestInstruction::test_()
{
    for(int i=0; i<100; i++) {
        //PROPIEDADES DE LA INTRUCCIÓN:

        //nombre de la instruccion
        //debe ser un nombre conocido:
        //      "": instrucción vacía; sin argumentos;
        //      "MA": establece modo asíncrono; sin argumentos;
        //      "MS": establece modo síncrono; sin argumentos;
        //      "M1": programa o desplaza el eje 1; con un argumento;
        //      "M2": programa o desplaza el eje 2; con un argumento;
        //      "MM": programa o desplaza ambos ejes; con dos argumentos;
        //      "ST": inicia el desplazamiento programado;
        //      "SP": detine el desplazamiento (si se está realizando).
        //valor por defecto: ""
        I->setName("");
        if(I->getName() != "") {
            CPPUNIT_ASSERT(false);
            return;
        }
        I->setName("MA");
        if(I->getName() != "MA") {
            CPPUNIT_ASSERT(false);
            return;
        }
        I->setName("MS");
        if(I->getName() != "MS") {
            CPPUNIT_ASSERT(false);
            return;
        }
        I->setName("M1");
        if(I->getName() != "M1") {
            CPPUNIT_ASSERT(false);
            return;
        }
        I->setName("M2");
        if(I->getName() != "M2") {
            CPPUNIT_ASSERT(false);
            return;
        }
        I->setName("MM");
        if(I->getName() != "MM") {
            CPPUNIT_ASSERT(false);
            return;
        }
        I->setName("ST");
        if(I->getName() != "ST") {
            CPPUNIT_ASSERT(false);
            return;
        }
        I->setName("SP");
        if(I->getName() != "SP") {
            CPPUNIT_ASSERT(false);
            return;
        }
        try {
            I->setName("other");
            CPPUNIT_ASSERT(false);
            return;
        }
        catch(Exception& E) {
            if(E.Message.str != "instruction name \"other\" should be known") {
                CPPUNIT_ASSERT(false);
                return;
            }
            //do nothing
        }

        //argumentos del comando
        //un argumento puede tener cualquier valor (incluso negativo)
        //valor por defecto: {}
        I->Args.Add(-1);
        I->Args.Add(0);
        I->Args.Add(1);
        if(I->Args.getCount() != 3 || I->Args[0] != -1 || I->Args[1] != 0 || I->Args[2] != 1) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //PROPIEDADES EN FORMATO TEXTO:

        //argumentos en formato texto
        if(I->Args.getItemsText(0).str != "-1" || I->Args.getItemsText(1).str != "0" || I->Args.getItemsText(2).str != "1") {
            CPPUNIT_ASSERT(false);
            return;
        }

        //instrucción en formato de linea de texto
        //  "Name Arg1 Arg2..."
        I->setText("MM 1 2");
        if(I->getText() != "MM 1 2") {
            CPPUNIT_ASSERT(false);
            return;
        }
        try {
            I->setText("MM 1");
            CPPUNIT_ASSERT(false);
            return;
        } catch(Exception& E) {
            if(E.Message.str != "setting instruction in text format in row 1 and column 5: second argument not found") {
                CPPUNIT_ASSERT(false);
                return;
            }
            //do nothing
        }
        try {
            I->setText("MM 1 2 3");
            CPPUNIT_ASSERT(false);
            return;
        } catch(Exception& E) {
            if(E.Message.str != "setting instruction in text format in row 1 and column 8: unexpected text: 3") {
                CPPUNIT_ASSERT(false);
                return;
            }
            //do nothing
        }

        //MÉTODOS ESTÁTICOS:

        //imprime una instrucción en una cadena de texto
        AnsiString S = "Instruction: ";
        TInstruction::Print(S, I);
        if(S != "Instruction: MM 1 2") {
            CPPUNIT_ASSERT(false);
            return;
        }
        //lee una instrucción en una cadena de texto
        I->setText("");
        if(I->getText().str != "") {
            CPPUNIT_ASSERT(false);
            return;
        }
        int j = 13;
        TInstruction::Read(I, S, j);
        if(I->getText() != "MM 1 2") {
            CPPUNIT_ASSERT(false);
            return;
        }

        //MÉTODOS PÚBLICOS:

        //construye una instrucción de posicionador
        TInstruction *I1 = new TInstruction();

        //asigna una instrucción
        *I1 = *I;
        if(I1->getName() != I->getName()) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //determines if an instruction is different to this instruction
        if(*I1 != *I) {
            CPPUNIT_ASSERT(false);
            return;
        }
        I1->setName("M1");
        if(!(*I1 != *I)) {
            CPPUNIT_ASSERT(false);
            return;
        }
        *I1 = *I;
        I1->Args.Add(3);
        if(!(*I1 != *I)) {
            CPPUNIT_ASSERT(false);
            return;
        }

        //libera la memoria dinámica
        delete I1;
    }

    CPPUNIT_ASSERT(true);
}

