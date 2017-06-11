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
/// @file RoboticPositionerTuner.h
/// @brief class tuner of RPs
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#ifndef ROBOTICPOSITIONERTUNER_H
#define ROBOTICPOSITIONERTUNER_H

#include "RoboticPositioner.h"

#include <vector>

using namespace Lists;

//---------------------------------------------------------------------------
//TuningParameters
//---------------------------------------------------------------------------

class TuningParameters
{
protected:
    double L01;
    double L13;
    double thetaO1;
    double theta_O3o;

public:
    //tuning parameters
    double x0;
    double y0;
    double getL01(void) const {return L01;}
    void setL01(double);
    double getL13(void) const {return L13;}
    void setL13(double);
    double getThetaO1(void) const {return thetaO1;}
    void seTthetaO1(double);
    double getTheta_O3o(void) const {return theta_O3o;}
    void setTheta_O3o(double);

    //get labels in row text format
    static string getLabelsRowText(void);
    //travel the labels in format row text
    static void travelLabelsRow(const string&, unsigned int& i);
    //read from a string
    //in format row text
    static void printRow(AnsiString& S, const TuningParameters TP);
    //print in a string
    //in format row text
    static void readRow(TuningParameters& TP, const AnsiString& S, int& i);

    TuningParameters(void) :
        L01(0), L13(0), thetaO1(0), theta_O3o(0), x0(0), y0(0) {}

    void clear(void);

    void set(const TuningParameters&);
    void set(double x0, double y0,
             double L01, double L13,
             double thetaO1, double theta_O3o);
    void get(TRoboticPositioner *RP) const;
    void set(const TRoboticPositioner *RP);

    string getRowText(void) const;
    string getAssignText(void) const;
};

//---------------------------------------------------------------------------
//Position
//---------------------------------------------------------------------------

class Position
{
public:
    //position angle of rotor 1 in sexagesimal degrees
    double rot1;
    //position angle of rotor 2 in sexagesimal degrees
    double rot2;
    //coordinates of point P3
    double x, y;

    //get labels in row text format
    static string getLabelsRowText(void);
    //travel the labels in format row text
    static void travelLabelsRow(const string&, unsigned int& i);
    //read from a string
    //in format row text
    static void printRow(AnsiString& S, const Position position);
    //print in a string
    //in format row text
    static void readRow(Position& position, const AnsiString& S, int& i);

    Position(void) : rot1(0), rot2(0), x(0), y(0) {;}

    Position& operator=(const Position&);
    bool operator!=(const Position&);

    void clear(void);

    void get(Position&) const;
    void set(const Position&);
    void set(double rot1, double rot2, double x, double y);

    string getRowText(void) const;
    string getAssignText(void) const;
};

//---------------------------------------------------------------------------
//PositionList
//---------------------------------------------------------------------------

class PositionList : public TItemsList<Position>
{
public:
    PositionList(void);

    PositionList& operator=(const PositionList&);
};

//---------------------------------------------------------------------------
//Error
//---------------------------------------------------------------------------

class Error
{
protected:
    //error function variables
    double ave;
    double max;
    int pos;

public:
    //tunning parameters which produces the error function values
    TuningParameters tuningParameters;
    //position which produces the maximun error
    Position worstPosition;
    //error function variables
    double getAve(void) const {return ave;} void setAve(double);
    double getMax(void) const {return max;} void setMax(double);
    int getPos(void) const {return pos;} void setPos(int);

    static string getLabelsRowText(void);
    static string getErrorFunctionVariablesLabelsRowText(void);
    static int comparePos(const Error error1, const Error error2);
    static void printRow(AnsiString&, const Error);
    static void readRow(Error&, const AnsiString&, int&);

    Error(void);
    Error& operator=(const Error&);
    void clear(void);
    void set(double ave, double max, int pos);

    string getAssignText(void) const;
    string getRowText(void) const;
    string getErrorFunctionVariablesRowText(void) const;
};

//---------------------------------------------------------------------------
//ErrorList
//---------------------------------------------------------------------------

class ErrorList : public TItemsList<Error> {
public:
    ErrorList(void);
    string getAvesRowText() const;
    string getMaxsRowText() const;
    string getPositionsRowText() const;
    void setText(const string&);
};

//---------------------------------------------------------------------------
//Measures
//---------------------------------------------------------------------------

class Measures {
public:
    //RP identifier
    int minibundle;

    //measured parameters
    double x0;      //P0.x in um
    double y0;      //P0.y in um
    double L01;     //L01 in um
    double L13;     //L13 in um
    double offset1; //thetaO1 = -offset1/180*M_PI;
    double offset2; //theta_O3o = offset2/180*M_PI + M_PI;

    //list of clusters (rot1, rot2, x, y)
    PositionList PL;

    static void read(Measures& measures, const string& str, unsigned int& i);

    Measures(void);

    Measures& operator=(const Measures&);

    void set(int minibundle, double x0,  double y0,
             double L01,  double L13,  double offset1,  double offset2);

    void getText(string&) const;
    void setText(const string&);

    //get the tuning parameters (x0, y0, L01, L13, offset1, offset2)
    void getTuningParameters(TRoboticPositioner *RP) const;
    //set the tuning parameters (x0, y0, L01, L13, offset1, offset2)
    void setTuningParameters(const TRoboticPositioner *RP);

    //determine the distances between P3 and (x, y)
    //and return the location of the worst position
    int determineError(Error& error, TRoboticPositioner *RP);
};

//---------------------------------------------------------------------------
//RoboticPositionerTuner
//---------------------------------------------------------------------------

class RoboticPositionerTuner
{
protected:
    TRoboticPositioner *RP;
    Measures *measures;

public:
    //tuning parameters
    //default values:
    //  (deltaS, deltaA) = (1./512, 1./512)
    //  (I, J, K, L, M, N) = (2, 2, 2, 2, 2, 2)
    double deltaS; //spatian increment
    double deltaA; //angular increment
    unsigned int I; //[x0 - I*deltaS, x0 + I*deltaS]
    unsigned int J; //[y0 - J*deltaS, y0 + J*deltaS]
    unsigned int K; //[L01 - K*deltaS, L01 + K*deltaS]
    unsigned int L; //[L13 - L*deltaS, L13 + L*deltaS]
    unsigned int M; //[thetaO1 - M*deltaA, thetaO1 + M*deltaA]
    unsigned int N; //[theta_O3o - N*deltaA, theta_O3o + N*deltaA]

    //switch for show process evolution
    //default value: false
    bool verbose;

    //build a robotic positioner tuner
    RoboticPositionerTuner(void);

    //set:
    //  (deltaS, deltaA) = (1./512, 1./512)
    //  (I, J, K, L, M, N) = (2, 2, 2, 2, 2, 2)
    void setFastTuningMode(void);
    //set:
    //  (deltaS, deltaA) = (1./1024, 1./2048)
    //  (I, J, K, L, M, N) = (4, 4, 4, 4, 4, 4)
    void setFineTuningMode(void);

    //search the tunning parameters of a RP using their measures
    //which minimize the average error
    void tune(TRoboticPositioner *RP, Measures *measures);
};

//---------------------------------------------------------------------------
#endif // ROBOTICPOSITIONERTUNER_H
