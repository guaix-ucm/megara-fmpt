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
/// @file RoboticPositionerTuner.cpp
/// @brief class tuner of RPs
/// @author Isaac Morales Durán
//---------------------------------------------------------------------------

#include "roboticpositionertuner.h"

#include <iostream> //std::cout, ios::fixed

//---------------------------------------------------------------------------
//TuningParameters
//---------------------------------------------------------------------------

void TuningParameters::setL01(double L01_)
{
    if(L01_ < 0)
        throw EImproperArgument("length L01 should be nonnegative");
    L01 = L01_;
}
void TuningParameters::setL13(double L13_)
{
    if(L13_ < 0)
        throw EImproperArgument("length L13 should be nonnegative");
    L13 = L13_;
}
void TuningParameters::seTthetaO1(double thetaO1_)
{
    if(thetaO1_ < -M_PI || M_PI <= thetaO1)
        throw EImproperArgument("angle thetaO1 should be in [-PI, PI)");
    thetaO1 = thetaO1_;
}
void TuningParameters::setTheta_O3o(double theta_O3o_)
{
    if(theta_O3o_ < 0 || M_2PI <= theta_O3o)
        throw EImproperArgument("angle theta_O3o should be in [0, 2*PI)");
    theta_O3o = theta_O3o_;
}

//get labels in row text format
string TuningParameters::getLabelsRowText(void)
{
    string str = "x0";
    str += " ";
    str += "y0";
    str += " ";
    str += "L01";
    str += " ";
    str += "L13";
    str += " ";
    str += "thetaO1";
    str += " ";
    str += "theta_O3o";

    return str;
}
//travel the labels in format row text
void TuningParameters::travelLabelsRow(const string& str, unsigned int& i)
{
    strTravelSeparatorsIfAny(str, i);
    strTravelLabel("x0", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("y0", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("L01", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("L13", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("thetaO1", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("theta_O3o", str, i);
}
//read from a string
//in format row text
void TuningParameters::printRow(AnsiString& S, const TuningParameters TP)
{
    S.str += TP.getRowText();
}

//print in a string
//in format row text
void TuningParameters::readRow(TuningParameters& TP, const AnsiString& S, int& i)
{
    if(i < 1 || S.Length() < i)
        throw EImproperArgument("index i should indicate a char in the string S");

    try {
        StrTravelSeparatorsIfAny(S, i);
        double x0;
        StrReadFloat(x0, S, i);

        StrTravelSeparatorsIfAny(S, i);
        double y0;
        StrReadFloat(y0, S, i);

        StrTravelSeparatorsIfAny(S, i);
        double L01;
        StrReadFloat(L01, S, i);

        StrTravelSeparatorsIfAny(S, i);
        double L13;
        StrReadFloat(L13, S, i);

        StrTravelSeparatorsIfAny(S, i);
        double thetaO1;
        StrReadFloat(thetaO1, S, i);

        StrTravelSeparatorsIfAny(S, i);
        double theta_O3o;
        StrReadFloat(theta_O3o, S, i);

        TP.set(x0, y0, L01, L13, thetaO1, theta_O3o);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading tuning parameters (P0, L01, L13, thetaO1, theta_O3o) in format row text: ");
        throw;
    }
}

void TuningParameters::clear(void)
{
    x0 = 0;
    y0 = 0;
    L01 = 0;
    L13 = 0;
    thetaO1 = 0;
    theta_O3o = 0;
}

void TuningParameters::set(const TuningParameters& TP)
{
    x0 = TP.x0;
    y0 = TP.y0;
    L01 = TP.L01;
    L13 = TP.L13;
    thetaO1 = TP.thetaO1;
    theta_O3o = TP.theta_O3o;
}

void TuningParameters::set(double x0_, double y0_,
                           double L01_, double L13_,
                           double thetaO1_, double theta_O3o_)
{
    x0 = x0_;
    y0 = y0_;
    L01 = L01_;
    L13 = L13_;
    thetaO1 = thetaO1_;
    theta_O3o = theta_O3o_;
}

void TuningParameters::get(TRoboticPositioner *RP) const
{
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    RP->getActuator()->setP0(TDoublePoint(x0, y0));
    RP->getActuator()->setL01(L01);
    RP->getActuator()->getArm()->setL13(L13);
    RP->getActuator()->setthetaO1(thetaO1);
    RP->getActuator()->settheta_O3o(theta_O3o);

}

void TuningParameters::set(const TRoboticPositioner *RP)
{
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    x0 = RP->getActuator()->getP0().x;
    y0 = RP->getActuator()->getP0().y;
    L01 = RP->getActuator()->getL01();
    L13 = RP->getActuator()->getArm()->getL13();
    thetaO1 = RP->getActuator()->getthetaO1();
    theta_O3o = RP->getActuator()->gettheta_O3o();
}

string TuningParameters::getRowText(void) const
{
    string str = floattostr(x0);
    str += " ";
    str += floattostr(y0);
    str += " ";
    str += floattostr(L01);
    str += " ";
    str += floattostr(L13);
    str += " ";
    str += floattostr(thetaO1);
    str += " ";
    str += floattostr(theta_O3o);

    return str;
}
string TuningParameters::getAssignText(void) const
{
    string str =    "x0: "              + floattostr(x0)        + " mm";
    str +=          "\r\ny0: "          + floattostr(y0)        + " mm";
    str +=          "\r\nL01: "         + floattostr(L01)       + " mm";
    str +=          "\r\nL13: "         + floattostr(L13)       + " mm";
    str +=          "\r\nthetaO1: "     + floattostr(thetaO1)   + " rad";
    str +=          "\r\ntheta_O3o: "   + floattostr(theta_O3o) + " rad";

    return str;
}

//---------------------------------------------------------------------------
//Position
//---------------------------------------------------------------------------

//get labels in row text format
string Position::getLabelsRowText(void)
{
    string str = "rot1";
    str += " ";
    str += "rot2";
    str += " ";
    str += "x";
    str += " ";
    str += "y";

    return str;
}
//travel the labels in format row text
void Position::travelLabelsRow(const string& str, unsigned int& i)
{
    strTravelSeparatorsIfAny(str, i);
    strTravelLabel("rot1", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("rot2", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("x", str, i);
    strTravelSeparators(str, i);
    strTravelLabel("y", str, i);
}

//read from a string a position cluster
//in format row text
void Position::printRow(AnsiString& S, const Position position)
{
    S.str += position.getRowText();
}

//print in a string a position cluster
//in format row text
void Position::readRow(Position& position, const AnsiString& S, int& i)
{
    if(i < 1 || S.Length() < i)
        throw EImproperArgument("index i should indicate a char in the string S");

    try {
        StrTravelSeparatorsIfAny(S, i);
        double rot1;
        StrReadFloat(rot1, S, i);

        StrTravelSeparators(S, i);
        double rot2;
        StrReadFloat(rot2, S, i);

        StrTravelSeparators(S, i);
        double x;
        StrReadFloat(x, S, i);

        StrTravelSeparators(S, i);
        double y;
        StrReadFloat(y, S, i);

        position.set(rot1, rot2, x, y);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading position cluster (rot1, rot2, x3, y3) in format row text: ");
        throw;
    }
}

Position& Position::operator=(const Position& position)
{
    rot1 = position.rot1;
    rot2 = position.rot2;
    x = position.x;
    y = position.y;

    return *this;
}

bool Position::operator!=(const Position& position)
{
    if(rot1 != position.rot1 || rot2 != position.rot2 || x != position.x || y != position.y)
        return true;
    return false;
}

void Position::clear(void)
{
    rot1 = 0;
    rot2 = 0;
    x = 0;
    y = 0;
}

void Position::get(Position& position) const
{
    position.rot1 = rot1;
    position.rot2 = rot2;
    position.x = x;
    position.y = y;
}

void Position::set(const Position& position)
{
    rot1 = position.rot1;
    rot2 = position.rot2;
    x = position.x;
    y = position.y;
}

void Position::set(double rot1_, double rot2_, double x_, double y_)
{
    rot1 = rot1_;
    rot2 = rot2_;
    x = x_;
    y = y_;
}

string Position::getRowText(void) const
{
    string str = floattostr(rot1);
    str += " ";
    str += floattostr(rot2);
    str += " ";
    str += floattostr(x);
    str += " ";
    str += floattostr(y);

    return str;
}

string Position::getAssignText(void) const
{
    string str =    "rot1: "    + floattostr(rot1)  + " º";
    str +=          "\r\nrot2: "  + floattostr(rot2)  + " º";
    str +=          "\r\nx: "     + floattostr(x)     + " um";
    str +=          "\r\ny: "     + floattostr(y)     + " um";

    return str;

}

//---------------------------------------------------------------------------
//PositionList
//---------------------------------------------------------------------------

PositionList::PositionList(void) :
    TItemsList<Position>(200, NULL, NULL, NULL, Position::printRow, Position::readRow)
{
}

PositionList& PositionList::operator=(const PositionList& L)
{
    //redimensiona la lista
    if(L.getCount() < getCount())
        Items.DelLast(getCount() - L.getCount());
    else if(L.getCount() > getCount())
        Items.NewLast(L.getCount() - getCount());

    //copia los elementos
    for(int i=0; i<getCount(); i++)
        Items[i] = L[i];

    return *this;
}

//---------------------------------------------------------------------------
//Error
//---------------------------------------------------------------------------

void Error::setAve(double ave_)
{
    if(ave_ <= 0)
        throw EImproperArgument("average error value (ave) should be nonnegative");

    ave = ave_;
}

void Error::setMax(double max_)
{
    if(max_ <= 0)
        throw EImproperArgument("maximun error value (max) should be nonnegative");

    max = max_;
}

void Error::setPos(int pos_)
{
    if(pos_ <= 0)
        throw EImproperArgument("number of positions to be into account (pos) should be nonnegative");

    pos = pos_;
}

string Error::getLabelsRowText(void)
{
    string str = TuningParameters::getLabelsRowText();
    str += " ";
    str += Position::getLabelsRowText();
    str += " ";
    str += " ave max pos";
    return str;
}

string Error::getErrorFunctionVariablesLabelsRowText(void)
{
    string str = "ave\tmax\tpos";

    return str;
}

int Error::comparePos(const Error error1, const Error error2)
{
    if(error1.pos < error2.pos)
        return -1;
    if(error1.pos > error2.pos)
        return 1;
    else return 0;
}

void Error::printRow(AnsiString& S, const Error error)
{
    S.str += error.getRowText();
}

void Error::readRow(Error& error, const AnsiString& S, int& i)
{
    //ADVERTENCIA: no se exige que la cadena de texto S sea imprimible,
    //de modo que cuando se quiera imprimir uno de sus caracteres,
    //si no es imprimible saldrá el caracter por defecto.

    //el índice i debería indicar a una posición de la cadena de texto S
    if(i<1 || S.Length()+1<i)
        throw EImproperArgument("index i should indicate a position in the string S");

    try {
        TuningParameters TP;
        TuningParameters::readRow(TP, S, i);

        Position position;
        Position::readRow(position, S, i);

        StrTravelSeparatorsIfAny(S, i);
        double ave;
        StrReadFloat(ave, S, i);

        StrTravelSeparatorsIfAny(S, i);
        double max;
        StrReadFloat(max, S, i);

        StrTravelSeparatorsIfAny(S, i);
        int pos;
        StrReadInt(pos, S, i);

        error.tuningParameters.set(TP);
        error.worstPosition.set(position);
        error.set(ave, max, pos);
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading error: ");
        throw;
    }
}

Error::Error(void) :
    ave(0), max(0), pos(0),
    tuningParameters(), worstPosition()
{
}

Error& Error::operator=(const Error& e)
{
    tuningParameters = e.tuningParameters;
    worstPosition = e.worstPosition;

    ave = e.ave;
    max = e.max;
    pos = e.pos;

    return *this;
}

void Error::clear(void)
{
    tuningParameters.clear();
    worstPosition.clear();

    ave = 0;
    max = 0;
    pos = 0;
}

void Error::set(double ave_, double max_, int pos_)
{
    ave = ave_;
    max = max_;
    pos = pos_;
}

string Error::getAssignText(void) const
{
    string str = "tuningParameters:";
    str += "\r\n" + StrIndent(tuningParameters.getAssignText()).str;

    str += "\r\nworstPosition:";
    str += "\r\n" + StrIndent(worstPosition.getAssignText()).str;

    str += "\r\nave: " + floattostr(ave) + " um";
    str += "\r\nmax: " + floattostr(max) + " um";
    str += "\r\npos: " + inttostr(pos) + " positions";

    return str;
}

string Error::getRowText(void) const
{
    string str = tuningParameters.getRowText();
    str += " " + worstPosition.getRowText();
    str += " " + floattostr(ave);
    str += " " + floattostr(max);
    str += " " + inttostr(pos);

    return str;
}

string Error::getErrorFunctionVariablesRowText(void) const
{
    int N = 4;
    string str = floattostr_fixed(ave, N);
    str += "\t" + floattostr_fixed(max, N);
    str += "\t" + inttostr(pos);

    return str;
}

//---------------------------------------------------------------------------
//ErrorList
//---------------------------------------------------------------------------

ErrorList::ErrorList(void) : TItemsList<Error>(256, NULL, NULL, NULL, Error::printRow, Error::readRow)
{
}

string ErrorList::getAvesRowText() const
{
    string str = floattostr(Items.getFirst().getAve());
    for(int i=1; i<getCount(); i++) {
        str += " ";
        str += floattostr(Items[i].getAve());
    }
    return str;
}
string ErrorList::getMaxsRowText() const
{
    string str = floattostr(Items.getFirst().getMax());
    for(int i=1; i<getCount(); i++) {
        str += " ";
        str += floattostr(Items[i].getMax());
    }
    return str;
}
string ErrorList::getPositionsRowText() const
{
    string str = inttostr(Items.getFirst().getPos());
    for(int i=1; i<getCount(); i++) {
        str += " ";
        str += inttostr(Items[i].getPos());
    }
    return str;
}

void ErrorList::setText(const string& str)
{
    try {
        unsigned int i = 0;

        //travel the header of the list
        TuningParameters::travelLabelsRow(str, i);
        Position::travelLabelsRow(str, i);

        strTravelSeparators(str, i);
        strTravelLabel("ave", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("max", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("pos", str, i);

        //read the items of the list
        strTravelSeparatorsIfAny(str, i);
        ErrorList EL;
        int i_ = int(i) + 1;
        ErrorList::ReadSeparated(&EL, str, i_);
        i = (unsigned int)(i_ - 1);

        //search unexpected text
        strTravelSeparatorsIfAny(str, i);
        if(i < str.length())
            throw EImproperArgument("unexpected text readin error list");

        //set the tampon variable
        *this = EL;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting error list: ");
        throw;
    }
}

//---------------------------------------------------------------------------
//Measures
//---------------------------------------------------------------------------

Measures::Measures(void) :
    minibundle(0), x0(0), y0(0), L01(0), L13(0), offset1(0), offset2(0),
    PL()
{
}

void Measures::read(Measures& measures, const string& str, unsigned int& i)
{
    if(str.length() < i)
        throw EImproperArgument("index i should indicate a position in the string str");

    try {
        strTravelSeparatorsIfAny(str, i);
        strTravelLabel("minibundle", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("x0", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("y0", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("L01", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("L13", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("offset1", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("offset2", str, i);

        int minibundle;
        double x0, y0, L01, L13, offset1, offset2;
        strTravelSeparators(str, i);
        strReadInt(minibundle, str, i);
        strTravelSeparators(str, i);
        strReadFloat(x0, str, i);
        strTravelSeparators(str, i);
        strReadFloat(y0, str, i);
        strTravelSeparators(str, i);
        strReadFloat(L01, str, i);
        strTravelSeparators(str, i);
        strReadFloat(L13, str, i);
        strTravelSeparators(str, i);
        strReadFloat(offset1, str, i);
        strTravelSeparators(str, i);
        strReadFloat(offset2, str, i);

        strTravelSeparators(str, i);
        strTravelLabel("rot1", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("rot2", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("x", str, i);
        strTravelSeparators(str, i);
        strTravelLabel("y", str, i);

        strTravelSeparatorsIfAny(str, i);
        PositionList PL;
        int i_ = int(i) + 1;
        PositionList::ReadSeparated(&PL, str, i_);
        i = (unsigned int)(i_ - 1);

        //search unexpected text
        strTravelSeparatorsIfAny(str, i);
        if(i < str.length())
            throw EImproperArgument("unexpected text readin measures");

        measures.set(minibundle, x0, y0, L01, L13, offset1, offset2);
        measures.PL = PL;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "reading measures in text format: ");
        throw;
    }
}

Measures& Measures::operator=(const Measures& measures)
{
    minibundle = measures.minibundle;
    x0 = measures.x0;
    y0 = measures.y0;
    L01 = measures.L01;
    L13 = measures.L13;
    offset1 = measures.offset1;
    offset2 = measures.offset2;

    PL = measures.PL;

    return *this;
}

void Measures::set(int minibundle_, double x0_,  double y0_,
                   double L01_,  double L13_,  double offset1_,  double offset2_)
{
    minibundle = minibundle_;
    x0 = x0_;
    y0 = y0_;
    L01 = L01_;
    L13 = L13_;
    offset1 = offset1_;
    offset2 = offset2_;
}

void Measures::getText(string& str) const
{
    str = "minibundle x0 y0 L01 L13 offset1 offset2";
    str += "\r\n"+inttostr(minibundle)+" "+floattostr(x0)+" "+floattostr(y0)+" "+floattostr(L01)+" "+floattostr(L13)+" "+floattostr(offset1)+" "+floattostr(offset2);
    str += "\r\nrot1 rot2 x y";
    for(int i=0; i<PL.getCount(); i++) {
        str += "\r\n";
        str += PL[i].getRowText();
    }
}
void Measures::setText(const string& str)
{
    try {
        Measures measures;
        unsigned int i = 0;
        Measures::read(measures, str, i);

        //search unexpected text
        strTravelSeparatorsIfAny(str, i);
        if(i < str.length())
            throw EImproperArgument("unexpected text reading measures cluster");

        *this = measures;
    }
    catch(Exception& E) {
        E.Message.Insert(1, "setting measures in text format");
        throw;
    }
}

//get the tuning parameters (x0, y0, L01, L13, offset1, offset2)
void Measures::getTuningParameters(TRoboticPositioner *RP) const
{
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    RP->getActuator()->setP0(TDoublePoint(x0/1000, y0/1000));
    RP->getActuator()->setL01(L01/1000);
    RP->getActuator()->getArm()->setL13(L13/1000);
    RP->getActuator()->setthetaO1(-(offset1/180*M_PI));
    RP->getActuator()->settheta_O3o(M_PI + (offset2/180*M_PI));
}

//set the tuning parameters (x0, y0, L01, L13, offset1, offset2)
void Measures::setTuningParameters(const TRoboticPositioner *RP)
{
    if(RP == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");

    x0 = RP->getActuator()->getP0().x*1000;
    y0 = RP->getActuator()->getP0().y*1000;
    L01 = RP->getActuator()->getL01()*1000;
    L13 = RP->getActuator()->getArm()->getL13()*1000;
    offset1 = -RP->getActuator()->getthetaO1()/M_PI*180;
    offset2 = (RP->getActuator()->gettheta_O3o() - M_PI)/M_PI*180;
}

//determine the distances between P3 and (x, y)
//and return the location of the worst position
int Measures::determineError(Error& error, TRoboticPositioner *RP)
{
    if(RP == NULL)
        throw EImproperCall("pointer RP should point to built robotic positioner");
    if(PL.getCount() <= 0)
        throw EImproperCall("there aren't positions for determine the error");

    error.clear();
    int i_worst;
    double sum = 0;
    for(int i=0; i<PL.getCount(); i++) {
        Position position = PL[i];

        //translate the angles to radians
        double theta_1 = position.rot1/180*M_PI;
        double theta___3 = position.rot2/180*M_PI;

        //set the positions
        RP->getActuator()->setAnglesRadians(theta_1, theta___3);
        //get the P3
        TDoublePoint P3 = RP->getActuator()->getArm()->getP3();
        //get the P
        TDoublePoint P;
        P.x = position.x/1000;
        P.y = position.y/1000;
        //calculate the error
        double e = Mod(P3 - P)*1000;
        //add the error
        sum += e;
        //actualize the maximun error
        if(e > error.getMax()) {
            error.setMax(e);
            error.worstPosition.set(position);
            i_worst = i;
        }
    }
    //calculate the averages
    error.setPos(PL.getCount());
    error.setAve(sum/error.getPos());

    //set the tuning parameters
    error.tuningParameters.set(RP);

    return i_worst;
}

//---------------------------------------------------------------------------
//RoboticPositionerTuner
//---------------------------------------------------------------------------

//build a robotic positioner tuner
RoboticPositionerTuner::RoboticPositionerTuner(void) :
    RP(NULL), measures(NULL), verbose(false)
{
    setFastTuningMode();
}

//set:
//  (deltaS, deltaA) = (1./512, 1./512)
//  (I, J, K, L, M, N) = (2, 2, 2, 2, 2, 2)
void RoboticPositionerTuner::setFastTuningMode(void)
{
    deltaS = 1./512;
    deltaA = 1./512;

    I = 2;
    J = 2;
    K = 2;
    L = 2;
    M = 2;
    N = 2;
}

//set:
//  (deltaS, deltaA) = (1./512, 1./512)
//  (I, J, K, L, M, N) = (2, 2, 2, 2, 2, 2)
void RoboticPositionerTuner::setFineTuningMode(void)
{
    deltaS = 1./1024;
    deltaA = 1./2048;
    I = 4;
    J = 4;
    K = 4;
    L = 4;
    M = 4;
    N = 4;
}

//search the tunning parameters of a RP using their measures
//which minimize the average error
void RoboticPositionerTuner::tune(TRoboticPositioner *RP_, Measures *measures_)
{
    if(RP_ == NULL)
        throw EImproperArgument("pointer RP should point to built robotic positioner");
    if(measures_ == NULL)
        throw EImproperArgument("pointer measures should point to built measures cluster");

    //point the external objects
    RP = RP_;
    measures = measures_;

    //get the initial values from the RP
    TDoublePoint P0ini = RP->getActuator()->getP0();
    double L01ini = RP->getActuator()->getL01();
    double L13ini = RP->getActuator()->getArm()->getL13();
    double thetaO1ini = RP->getActuator()->getthetaO1();
    double theta_O3oIni = RP->getActuator()->gettheta_O3o();

    //minimun parameters
    TDoublePoint P0min;
    double L01min;
    double L13min;
    double thetaO1min;
    double theta_O3oMin;

    Error error;
    double errorAveMin = DBL_MAX;
    double errorMaxMax = 0;

    //search the value of the parameters which produces minimun error
    int i, j, k, l, m, n;
    for(i = -int(I); i<=int(I); i++) {
        if(verbose) {
            //compose the i_str
            string i_str = inttostr(i + I + 1);
            while(i_str.length() < 2)
                i_str.insert(i_str.begin(), '0');
            //compose the I_str
            string I_str = inttostr(2*I + 1);
            //print the init of the next line
            std::cout << std::endl;
            std::cout << "tuning " << i_str << "/" << I_str << ": ";
            std::flush(std::cout);
        }
        //perform the searching
        for(j = -int(J); j<=int(J); j++) {
            TDoublePoint P0 = P0ini + TDoublePoint(i*deltaS, j*deltaS);
            RP->getActuator()->setP0(P0);

            for(k=-int(K); k<=int(K); k++) {
                double L01 = L01ini + k*deltaS;
                RP->getActuator()->setL01(L01);

                for(l=-int(L); l<=int(L); l++) {
                    double L13 = L13ini + l*deltaS;
                    RP->getActuator()->getArm()->setL13(L13);

                    for(m=-int(M); m<=int(M); m++) {
                        double thetaO1 = thetaO1ini + m*deltaA;
                        RP->getActuator()->setthetaO1(thetaO1);

                        for(n=-int(N); n<=int(N); n++) {
                            double theta_O3o = theta_O3oIni + n*deltaA;
                            RP->getActuator()->settheta_O3o(theta_O3o);

                            measures->determineError(error, RP);

                            if(error.getAve() < errorAveMin) {
                                errorAveMin = error.getAve();
                                P0min = P0;
                                L01min = L01;
                                L13min = L13;
                                thetaO1min = thetaO1;
                                theta_O3oMin = theta_O3o;
                            }
                            if(error.getMax() > errorMaxMax)
                                errorMaxMax = error.getMax();
                        }
                    }
                }
            }
            //print the progress, if verbose
            if(verbose) {
                std::cout << "*";
                std::flush(std::cout);
            }
        }
    }

    RP->getActuator()->setP0(P0min);
    RP->getActuator()->setL01(L01min);
    RP->getActuator()->getArm()->setL13(L13min);
    RP->getActuator()->setthetaO1(thetaO1min);
    RP->getActuator()->settheta_O3o(theta_O3oMin);
}
