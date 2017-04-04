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
//File: testTelescopeProjectionModel.cpp
//Content: test for the module TelescopeProjectionModel
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#include "testTelescopeProjectionModel.h"
#include "../src/FiberMOSModelConstants.h" //GTC_R
#include "../src/Constants.h" //M_2PI
//#include "cppunit_assert_emulator.h" //Qt only.

#include <cmath> //abs
#include <vector>
//#include <stdlib.h>
#include <limits> //std::numeric_limits

#include "../src/TelescopeProjectionModel.h"

using namespace Models;

//---------------------------------------------------------------------------

//commented in Qt:
CPPUNIT_TEST_SUITE_REGISTRATION(TestTelescopeProjectionModel);

//overide setUp(), init data, etc
void TestTelescopeProjectionModel::setUp() {
}

//overide tearDown(), free allocated memory, etc
void TestTelescopeProjectionModel::tearDown() {
}

//---------------------------------------------------------------------------

void TestTelescopeProjectionModel::test_ProjectionCircle()
{
    for(int i=0; i<1000; i++) {
        try {
            TProjectionCircle *PC1 = new TProjectionCircle();
            if(PC1->O != TDoublePoint(0, 0) || PC1->getR() != 0) {
                CPPUNIT_ASSERT(false);
                return;
            }
            TProjectionCircle *PC2 = new TProjectionCircle(TDoublePoint(-1, 1), 1);
            if(PC2->O != TDoublePoint(-1, 1) || PC2->getR() != 1) {
                CPPUNIT_ASSERT(false);
                return;
            }
            TProjectionCircle *PC3 = new TProjectionCircle(PC2);
            if(PC2->O != PC3->O || PC2->getR() != PC3->getR()) {
                CPPUNIT_ASSERT(false);
                return;
            }

            if(PC2->PointIsInDomain(TDoublePoint(0, 0))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(PC2->PointIsInDomain(TDoublePoint(-0.99, 0))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(!PC2->PointIsInDomain(TDoublePoint(-1, 0))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(PC2->PointIsInDomain(TDoublePoint(0, 0.99))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(!PC2->PointIsInDomain(TDoublePoint(0, 1))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(!PC2->PointIsInDomain(TDoublePoint(-1, 1))) {
                CPPUNIT_ASSERT(false);
                return;
            }

            if(!PC2->PointIsntInDomain(TDoublePoint(0, 0))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(!PC2->PointIsntInDomain(TDoublePoint(-0.99, 0))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(PC2->PointIsntInDomain(TDoublePoint(-1, 0))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(!PC2->PointIsntInDomain(TDoublePoint(0, 0.99))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(PC2->PointIsntInDomain(TDoublePoint(0, 1))) {
                CPPUNIT_ASSERT(false);
                return;
            }
            if(PC2->PointIsntInDomain(TDoublePoint(-1, 1))) {
                CPPUNIT_ASSERT(false);
                return;
            }

            //get the limits from random points
            double xmin = DBL_MAX;
            double xmax = -DBL_MAX;
            double ymin = DBL_MAX;
            double ymax = -DBL_MAX;
            double Rmin = DBL_MAX;
            double Rmax = -DBL_MAX;
            double thetamin = DBL_MAX;
            double thetamax = -DBL_MAX;
            for(int i=0; i<1000; i++) {
                TDoublePoint Q = PC2->RandomPoint();

                //actualiza los límites cartesianos
                if(Q.x < xmin)
                    xmin = Q.x;
                if(Q.x > xmax)
                    xmax = Q.x;
                if(Q.y < ymin)
                    ymin = Q.y;
                if(Q.y > ymax)
                    ymax = Q.y;

                //actualiza los límites polares
                double R = Mod(Q - PC2->O);
                if(R < Rmin)
                    Rmin = R;
                if(R > Rmax)
                    Rmax = R;
                double theta = ArgPos(Q - PC2->O);
                if(theta < thetamin)
                    thetamin = theta;
                if(theta > thetamax)
                    thetamax = theta;
            }
            //check the limits
            double aux = xmin - (PC2->O.x - PC2->getR());
            if(aux < 0 ||  0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }
            aux = PC2->O.x + PC2->getR() - xmax;
            if(aux < 0 ||  0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }
            aux = ymin - (PC2->O.y - PC2->getR());
            if(aux < 0 ||  0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }
            aux = PC2->O.y + PC2->getR() - ymax;
            if(aux < 0 ||  0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }
            aux = Rmin;
            if(aux < 0 || 0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }
            aux = PC2->getR() - Rmax;
            if(aux < 0 || 0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }
            aux = thetamin;
            if(aux < 0 || 0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }
            aux = M_2PI - thetamax;
            if(aux < 0 || 0.1 < aux) {
                CPPUNIT_ASSERT(false);
                return;
            }

            delete PC3;
            delete PC2;
            delete PC1;
        }
        catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}

void TestTelescopeProjectionModel::test_FocalPlane()
{
    for(int i=0; i<1000; i++) {
        try {
            TFocalPlane *FP1 = new TFocalPlane();
            if(FP1->getO() != TDoublePoint(0, 0) || FP1->getR() != GTC_R || FP1->getProjectionCircle() != NULL ||
                    FP1->ProjectionPointList.getCount() != 0 || FP1->InnerProjectionPointList.getCount() != 0) {
                CPPUNIT_ASSERT(false);
                return;
            }

            FP1->setR(1);
            TProjectionPoint *PP = new TProjectionPoint();
            PP->x = 1;
            PP->y = 1;
            FP1->ProjectionPointList.Add(PP);
            PP = new TProjectionPoint();
            PP->x = 1;
            PP->y = -1;
            FP1->ProjectionPointList.Add(PP);
            PP = new TProjectionPoint();
            PP->x = -1;
            PP->y = -1;
            FP1->ProjectionPointList.Add(PP);
            FP1->InnerProjectionPointList.Add(FP1->ProjectionPointList.GetPointer(0));
            FP1->InnerProjectionPointList.Add(FP1->ProjectionPointList.GetPointer(2));

            if(FP1->getO() != TDoublePoint(0, 0) || FP1->getR() != 1 || FP1->getProjectionCircle() != NULL ||
                    FP1->ProjectionPointList.getCount() != 3 || FP1->InnerProjectionPointList.getCount() != 2) {
                CPPUNIT_ASSERT(false);
                return;
            }

            TFocalPlane *FP2 = new TFocalPlane();
            FP2->CopyFocalPlane(FP1);
            if(FP2->getO() != TDoublePoint(0, 0) || FP2->getR() != 1 || FP2->getProjectionCircle() != NULL ||
                    FP1->ProjectionPointList.getCount() != 3 || FP1->InnerProjectionPointList.getCount() != 2) {
                CPPUNIT_ASSERT(false);
                return;
            }

            FP2->Clear();
            if(FP2->getO() != TDoublePoint(0, 0) || FP2->getR() != 1 || FP2->getProjectionCircle() != NULL ||
                    FP2->ProjectionPointList.getCount() != 0 || FP2->InnerProjectionPointList.getCount() != 0) {
                CPPUNIT_ASSERT(false);
                return;
            }

            delete FP2;
            delete FP1;
        }
        catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}

void TestTelescopeProjectionModel::test_TelescopeProjectionModel()
{
    for(int i=0; i<1000; i++) {
        try {
            TTelescopeProjectionModel *TPM = new TTelescopeProjectionModel();

            if(TPM->getTile() != NULL ||
                    TPM->getRA() != 0 || TPM->getDEC() != 0 || TPM->getPA() != 0 ||
                    TPM->getR() != 850 || TPM->getangview() != 1031.305) {
                CPPUNIT_ASSERT(false);
                return;
            }

            TTile Tile;
            Tile.setId(1);

            double RA, DEC;
            TPM->Deproject(RA, DEC, TDoublePoint(494.51774895884478, 0));
            if(fabs(RA - 0.0029088902912790199) > ERR_NUM || fabs(DEC - 0) > ERR_NUM) {
                CPPUNIT_ASSERT(false);
                return;
            }

            TPM->Deproject(RA, DEC, TDoublePoint(GTC_R, 0));
            if(fabs(RA - 0.00076523770927792458) > ERR_NUM || fabs(DEC - 0) > ERR_NUM) {
                CPPUNIT_ASSERT(false);
                return;
            }

            //Remember:
            //  class TSkyPointList : public TItemsList<TSkyPoint*>
            //  Tile.getR_() default value: 2.9088902912790199E-3 rad (= 10 arcmin)

            TSkyPoint SP1;
            SP1.setRowText("1 0.0007652378 0");
            TSkyPoint SP2;
            SP2.setRowText("2 0 -0.0007652378");
            TSkyPoint SP3;
            SP3.setRowText("3 -0.0007652378 0");
            TSkyPoint SP4;
            SP4.setRowText("4 0 0.0007652378");

            Tile.SkyPointList.Add(&SP1);
            Tile.SkyPointList.Add(&SP2);
            Tile.SkyPointList.Add(&SP3);
            Tile.SkyPointList.Add(&SP4);

            TPM->Point(&Tile);
            TPM->Project();
            if(TPM->ProjectionPointList.getCount() != 4 || TPM->InnerProjectionPointList.getCount() != 0) {
                CPPUNIT_ASSERT(false);
                return;
            }

            SP1.setRowText("1 0.0007652376 0");
            SP2.setRowText("2 0 -0.0007652376");
            SP3.setRowText("3 -0.0007652376 0");
            SP4.setRowText("2 0 0.0007652376");

            TPM->Project();
            if(TPM->ProjectionPointList.getCount() != 4 || TPM->InnerProjectionPointList.getCount() != 4) {
                CPPUNIT_ASSERT(false);
                return;
            }

            delete TPM;
        }
        catch(...) {
            CPPUNIT_ASSERT(false);
            return;
        }
    }

    CPPUNIT_ASSERT(true);
}
