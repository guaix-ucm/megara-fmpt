#include "GenerateFrames.h"

#include <QTimer>
#include <QPainter>
#include <QDebug>

QPicture GenerateFrames::printFMM()
{
    if(FMM == NULL)
        throw EImproperArgument("The pointer FMM should point to built Fiber MOS Model");

    //paint the FMM in the picture
    //enable painter and set picture it
    QPainter painter;
    painter.begin(PloterShapes.getPicture());

    for(int i=0; i<FMM->RPL.getCount(); i++) {
        TRoboticPositioner *RP = FMM->RPL[i];

        //paint the cilinder of the RP
        PloterShapes.Circunference(RP->getActuator()->getP0(), RP->getActuator()->getL03max(), painter);

        //paint the arm of the RP
        TArm *Arm = RP->getActuator()->getArm();
        for(int j=0; j<Arm->getContour().getCount(); j++) {
            TContourFigure *F = Arm->getContour().Get(j);

            //si la figura es de tipo TSegment
            if(typeid(*F) == typeid(TSegment)) {
                TSegment *S = (TSegment*)F;
                TDoublePoint Pa = S->getPa();
                TDoublePoint Pb = S->getPb();
                PloterShapes.Segment(Pa, Pb, painter);
            }
            //si la figura es de tipo TArc
            else if(typeid(*F) == typeid(TArc)) {
                TArc *A = (TArc*)F;
                TDoublePoint Pa = A->getPa();
                TDoublePoint Pb = A->getPb();
                TDoublePoint Pc = A->getPc();
                double R = A->getR();
                PloterShapes.Arc(Pa, Pb, Pc, R, painter);
            }
            //si F apunta a nulo
            else if(F == NULL) {
                //no hace nada
            }
            else {
                //figura geométrica de tipo desconocido
                throw EImpossibleError("geometrical figure type unknown");
            }

            PloterShapes.Circunference(Arm->getP3(), 0.75, painter);
        }
    }

    //disable painter
    painter.end();
    //ui->imageLabel->setPicture(*(PloterShapes.getPicture()));
    return *(PloterShapes.getPicture());

}

void GenerateFrames::generatePPFrames()
{

    //Creates Images PP
    for(int i=0; i<PP->getCount(); i++) {

        TMessageList *ML = PP->GetPointer(i);

        //configure the FMM
        FMM->RPL.clearInstructions();
        for(int j=0; j< ML->getCount(); j++) {
            TMessageInstruction *MI = ML->GetPointer(j);
            FMM->RPL.setInstruction(MI->getId(), MI->Instruction);
        }

        //execute the instructions
        double t = 0;
        double T = FMM->RPL.getTdis();
        //double deltat = 1./25000; <---- Wow ! Too short / Too many frames !!
        double deltat = 100;
        int k = 1;

        while(t < T) {

            //move the FMM to time t
            FMM->RPL.move(t);

            //generates the image
            QPicture frame = printFMM();

            //save the image
            QString indexframei = QString("%1").arg(i, 2, 10, QChar('0'));
            QString indexframek = QString("%1").arg(k, 3, 10, QChar('0'));
            frame.save(pathSequence+"APP_"+indexframei+"_"+indexframek+".fmpt");

            //increase t
            t += deltat;
            k++;
        }

        qDebug() << "PP["<< i <<"]"<<"  "<< "TDis:"<< T<<"  "<< "Frames:"<< k;

    }
}

void GenerateFrames::generateDPFrames()
{

    // Creates Images DP
    for(int i=0; i<DP->getCount(); i++) {

        TMessageList *ML = DP->GetPointer(i);

        //configure the FMM
        FMM->RPL.clearInstructions();
        for(int j=0; j< ML->getCount(); j++) {
            TMessageInstruction *MI = ML->GetPointer(j);
            FMM->RPL.setInstruction(MI->getId(), MI->Instruction);
        }

        //execute the instructions
        double t = 0;
        double T = FMM->RPL.getTdis();
        //double deltat = 1./25000; <---- Wow ! Too short / Too many frames !!
        double deltat = 100;
        int k = 1;

        while(t < T) {

            //move the FMM to time t
            FMM->RPL.move(t);

            //generates the image
            QPicture frame = printFMM();

            //save the image
            QString indexframei = QString("%1").arg(i, 2, 10, QChar('0'));
            QString indexframek = QString("%1").arg(k, 3, 10, QChar('0'));
            frame.save(pathSequence+"BDP_"+indexframei+"_"+indexframek+".fmpt");

            //increase t
            t += deltat;
            k++;
        }
        qDebug() << "DP["<< i <<"]"<<"  "<< "TDis:"<< T<<"  "<< "Frames:"<< k;
    }
}

