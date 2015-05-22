// Copyright (c) 2014-2015 Isaac Morales Durán. All rights reserved.
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
//File: RoboticPositionerList2.h
//Content: RP list with hash table
//Last update: 13/02/2015
//Author: Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERLIST2_H
#define ROBOTICPOSITIONERLIST2_H

#include "RoboticPositionerList1.h"

//---------------------------------------------------------------------------

//namespace for models
namespace Models {

//---------------------------------------------------------------------------

//class RP list with hash table
class TRoboticPositionerList2 : public TRoboticPositionerList1 {
protected:
    //MAPPING INTERVAL:

    double p_xmin;
    double p_xmax;
    double p_ymin;
    double p_ymax;

    //CELL MATRIX OF THE MAP:

    //cell matrix
    TPointersList<TPointersList<TItemsList<TRoboticPositioner*> > > p_Cells;

    int p_J;

    //DERIVED PARAMETERS:

    double p_qx;
    double p_qy;

    //ASSIMILATION METHODS:

    //calculates the sizing of a cell (qx, qy)
    void calculateSize(void);

public:
    //MAPPING INTERVAL:

    //lower limit of the interval x of the map
    //shall be lower than upper limit
    //default value: -106.08 mm
    double getxmin(void) const {return p_xmin;}
    void setxmin(double);
    //upper limit of the interval x of the map
    //shall be upper than lower limit
    //default value: 106.08 mm
    double getxmax(void) const {return p_xmax;}
    void setxmax(double);
    //lower limit of the interval y of the map
    //shall be lower than upper limit
    //default value: -102 mm
    double getymin(void) const {return p_ymin;}
    void setymin(double);
    //upper limit of the interval y of the map
    //shall be upper than lower limit
    //default value: 102 mm
    double getymax(void) const {return p_ymax;}
    void setymax(double);

    //WARNING: the mapping interval shall contains the focal plane,
    //becuase only will be processed the points in the focal plane.

    //CELL MATRIX OF THE MAP:

    //number of rows of the grid
    //shall be upper zero
    //default value: 50
    int getI(void) const {return p_Cells.getCount();} void setI(int);
    //number of columns of the grid
    //shall be upper zero
    //default value: 52
    int getJ(void) const {return p_J;} void setJ(int);

    //ADVICE: cell matriz dimensions shall be such that the diagonal of each cell
    //doesnot exceed L, so that the side of each cell should be at most:
    //      qmax = L/sqrt(2.) - ERR_NUM,
    //since:
    //  - when the diagonal of a cell is below L,
    //    the maximun number of RPs by cell is 3;
    //  - when the sizing of a cell is lower, is most probably that
    //    a point in the exclusive area is in a cell with one RP only.

    //DERIVATED PARAMETERS:

    //base of a cell
    //      qx = (xmax - xmin)/J
    //default value: 4.08 mm
    double getqx(void) const {return p_qx;}
    //hight of a cell
    //      qy = (ymax - ymin)/I
    //default value: 4.08 mm
    double getqy(void) const {return p_qy;}

    //PROPERTIES IN TEXT FORMAT:

    AnsiString getxminText(void) const;
    void setxminText(const AnsiString&);
    AnsiString getxmaxText(void) const;
    void setxmaxText(const AnsiString&);
    AnsiString getyminText(void) const;
    void setyminText(const AnsiString&);
    AnsiString getymaxText(void) const;
    void setymaxText(const AnsiString&);

    AnsiString getIText(void) const;
    void setIText(const AnsiString&);
    AnsiString getJText(void) const;
    void setJText(const AnsiString&);

    AnsiString getqxText(void) const;
    AnsiString getqyText(void) const;

    //SET OF PROPERTIES IN TEXT FORMAT:

    //mapping interval
    //in format assign of text
    AnsiString getIntervalText(void) const;
    void setIntervalText(const AnsiString&);
    //cells ofthe map
    //in format assign of text
    AnsiString getCellsText(void) const;
    void setCellsText(const AnsiString&);
    //size of cell
    //in format assign of text
    AnsiString getSizeText(void);

    //instance of the RP map
    //in format assign of text
    AnsiString getInstanceMapText(void) const;

    //-------------------------------------------------------------------
    //STATIC METHODS FOR INDIVIDUAL PROPERTIES:

    //read xmin in assignment text format
    static void  readxminText(double &xmin, const AnsiString &S, int &i);
    //read xmax in assignment text format
    static void  readxmaxText(double &xmax, const AnsiString &S, int &i);
    //read ymin in assignment text format
    static void  readyminText(double &ymin, const AnsiString &S, int &i);
    //read umax in assignment text format
    static void  readymaxText(double &ymax, const AnsiString &S, int &i);

    //read I in assignment text format
    static void  readIText(int &I,  const AnsiString &S, int &i);
    //read J in assignment text format
    static void  readJText(int &J, const AnsiString &S, int &i);

    //STATIC METHODS FOR SETS OF PROPERTIES:

    //print the mapping interval of a RP map
    //in a text string in assign format
    static void  printInterval(AnsiString &S,
                               const TRoboticPositionerList2 *RPL);
    //read the mapping interval of a RP map
    //in a text string in assign format
    static void  readInterval(TRoboticPositionerList2 *RPL,
                              const AnsiString &S, int &i);

    //print the cells of a RP map
    //in a text string in assign format
    static void  printCells(AnsiString &S,
                            const TRoboticPositionerList2 *RPL);
    //read the cells of a RP map
    //in a text string in assign format
    static void  readCells(TRoboticPositionerList2 *RPL,
                           const AnsiString &S, int &i);

    //-------------------------------------------------------------------
    //BUILDING, COPY AND DESTROYING METHODS:

    //build a RP list
    TRoboticPositionerList2(void);

    //copy the cell matrix
    void copyCells(const TRoboticPositionerList2*);
    //copy the RP map of a RPL
    void copyMap(const TRoboticPositionerList2*);

    //clone a RPL including:
    //  (Tolerance, Sizing, Area, Graphics, Map)
    void Clone(const TRoboticPositionerList2*);

    //WARNING: inherited methods must be redefined withidentical name
    //for avoid errors.

    //build a clon of a RPL
    TRoboticPositionerList2(const TRoboticPositionerList2*);

    //MAP METHODS:

    //set the limmits of the mapping interval
    void setMapInterval(double xmin, double xmax, double ymin, double ymax);
    //set the dimensions of the cell matrix
    void setMapDimensions(int I, int J);
    //set the content of the cell matrix
    void setMapCells(TPointersList<TPointersList<TItemsList<TRoboticPositioner*> > > &_Cells);

    //maps the RPs in the mapping interval
    void map(void);
    //automatically:
    //      1. stablish the appropriate limits of the mapping interval
    //      2. stablish the appropriate dimensions of the cell matrix
    //      3. maps the RPs in the mapping interval
    void generateMap(void);

    //assimilates the configuration of RPs executing:
    //      calculateSPMComponents();
    //      assimilateSizing();
    //      generateMap();
    void assimilate(void);

    //assimilates all RPs of the RPL to a given RP
    void apply(const TRoboticPositioner*);

    //transform x in j by default
    //      x = floor((x - xmin)/qx)
    int jfloor(double x);
    //transform y in i by default
    //      y = floor((y - ymin)/qy)
    int ifloor(double y);
    //transform x in j by excess
    //      x = ceil((x - xmin)/qx)
    int jceil(double x);
    //transform y in i by excess
    //      y = ceil((y - ymin)/qy)
    int iceil(double y);

    //access to the RP list of the cell which contains a point
    TItemsList<TRoboticPositioner*> *roboticPositionersCell(TDoublePoint P);

    //search the RPs whose scope is a point
    void searchPositioners(TItemsList<TRoboticPositioner*> &RPs,
                           TDoublePoint P);
};

//How to use the class TRoboticPositionerList2:
//      //set the mapping limits
//      RPL->SetLimits(-RPL->R, RPL->R, -RPL->R, RPL->R);
//      //setthedimensions of the cell matrix
//      RPL->SetDimensions(floor(2*RPL->R/(L/2)), floor(2*RPL->R/(L/2)));
//      //maps the RPs ofthe RPL
//      RPL->Map(RPL);
//      //determines the RPs whose scope is a point
//      RPL->SearchPositioners(RPs, P);

//---------------------------------------------------------------------------

} //namespace Models

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONERLIST2_H
