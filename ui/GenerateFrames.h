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
//File: GenerateFrames.h
//Content: gráfical classes and grphical functions
//Autor: José Enrique Ruíz
//---------------------------------------------------------------------------

#ifndef GENERATEFRAMES_H
#define GENERATEFRAMES_H

#include "../megarafmpt/src/MotionProgram.h"
#include "../megarafmpt/src/FiberMOSModel.h"
#include "../megarafmpt/ui/PloterShapes.h"

using namespace Graphics;

class GenerateFrames
{
public:

    TPloterShapes PloterShapes;
    TFiberMOSModel *FMM;
    TMotionProgram *PP;
    TMotionProgram *DP;
    QString pathSequence;

    void generatePPFrames();
    void generateDPFrames();

private:
    QPicture printFMM();

};

//---------------------------------------------------------------------------
#endif // GENERATEFRAMES_H
