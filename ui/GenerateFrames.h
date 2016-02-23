#ifndef GENERATEFRAMES_H
#define GENERATEFRAMES_H

#include "../megarafmpt/src/MotionProgram.h"
#include "../megarafmpt/src/FiberMOSModel2.h"
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



#endif // GENERATEFRAMES_H
